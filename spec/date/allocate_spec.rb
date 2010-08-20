require File.expand_path('../../spec_helper', __FILE__)

describe "Date.allocate" do
  it "should not be defined" do
    proc{Date.allocate}.should raise_error
  end
end
